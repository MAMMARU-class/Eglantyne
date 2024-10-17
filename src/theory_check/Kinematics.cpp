#include "Kinematics.h"
using namespace Eigen;

// set ptrs for lists to endeffecer from body
std::vector<Link*> Kinematics::showFromBody(Link* link){
    if (link->parent() == nullptr){
        return {};
    }

    std::vector<Link*> link_list = showFromBody(link->parent());
    link_list.push_back(link);
    return link_list;
}

// forward kinematics. calclate every link's (from body to endeffect) P_w nad R_w
void Kinematics::forward(Link* link){
    if (link->parent() == nullptr){
        Matrix4d RootMat;
        RootMat.block<3,1>(0,0) = MatrixXd::Zero(3,1);
        RootMat.block<3,3>(0,1) = Matrix3d::Identity();
        RootMat(3,0) = 1;
        RootMat.block<1,3>(3,1) = MatrixXd::Zero(1,3);
        link->setState_w(RootMat);
        return;
    }

    forward(link->parent());
    Matrix4d State_w;
    // multiple parent HT matrix with target local states
    State_w = link->parent()->getHTMat() * link->getState_l();
    link->setState_w(State_w);
    return;
}

void Kinematics::inverse(Link* link, Vector3d P_ref, Matrix3d R_ref){
    std::vector<Link*> link_list = showFromBody(link);
    int link_size = link_list.size();

    VectorXd q_vec(link_size);
    int vec_id = 0;
    for (const auto& link : link_list){
        q_vec(vec_id) = link->getq();
        vec_id++;
    }

    for (int i=10; i>0; i--){
        forward(link);
        // err : 6 dementional vector (P, w)
        VectorXd err = calcerr(link, P_ref, R_ref); // 6 dementional
        if (err.norm() < std::numeric_limits<double>::epsilon()){
            return;
        }else{
            MatrixXd Jacobi = calcJacobi(link_list); // 6*link_size matrix
            double lambda = 0.5;
            VectorXd deltaq(link_size);
            deltaq = lambda * Jacobi.completeOrthogonalDecomposition().pseudoInverse() * err;
            q_vec += 180.0 / M_PI * deltaq;
            // setQ(q_vec, link_list);
            int link_id = 0;
            for (const auto& link : link_list){
                link->setq(q_vec(link_id));
                link_id++;
            }
        }
    }
}

// sub calculer

MatrixXd Kinematics::calcJacobi(std::vector<Link*> link_list){
    MatrixXd Jacobi(6, link_list.size());
    int row = 0;
    for (const auto& link : link_list){
        Vector3d A_w = link->getA_w();
        // P_w_n - P_w_i
        Vector3d P_i_n = link_list.back()->getP_w() - link->getP_w();
        
        MatrixXd check(3,2);
        check.block<3,1>(0,0) = A_w;
        check.block<3,1>(0,1) = P_i_n;

        Jacobi.block<3,1>(0,row) = A_w.cross(P_i_n);
        Jacobi.block<3,1>(3,row) = A_w;
        row++;
    }
    cout << Jacobi << "\n" << endl;
    return Jacobi;
}

// compare endeffercor P_w, R_w to P_ref, R_ref
VectorXd Kinematics::calcerr(Link* link, Vector3d P_ref, Matrix3d R_ref){
    VectorXd errVec(6);
    
    Vector3d P_err = P_ref - link->getP_w();
    Matrix3d R_now = link->getR_w();
    Matrix3d R_err = R_now.inverse() * R_ref;
    Vector3d w_err = R_now * rot2omega(link, R_err);

    errVec.segment(0,3) = P_err;
    errVec.segment(3,3) = w_err;
    return errVec;
}

// convert rotation error to angluler velocity
Vector3d Kinematics::rot2omega(Link* link, Matrix3d R_ref){
    Vector3d w;
    
    Matrix3d R_now = link->getR_w();
    Vector3d el;
    el << R_now(2,1) - R_now(1,2),
          R_now(0,2) - R_now(2,0),
          R_now(1,0) - R_now(0,1);
    double norm_el = el.norm();

    if (norm_el > std::numeric_limits<double>::epsilon()){
        return link->getq() / norm_el * el;
    }else if (R_now(0,0) > 0 && R_now(1,1) > 0 && R_now(2,2) > 0){
        return Vector3d::Zero();
    }else{
        Vector3d v;
        v << R_now(0,0) + 1,
             R_now(1,1) + 1,
             R_now(2,2) + 1;
        return M_PI / 2 * v;
    }
}

void setQ(VectorXd q_vec, std::vector<Link*> link_list){
    int link_id = 0;
    for (const auto& link : link_list){
        link->setq(q_vec(link_id));
        link_id++;
    }
}
