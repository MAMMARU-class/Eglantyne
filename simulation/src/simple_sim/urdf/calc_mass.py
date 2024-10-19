motor_mass = 27
mass_base = 726 / 53
masses = []

while True:
    motor_n = input("input number of motor : ")
    if motor_n == " ":
        m = 0
        for mass in masses:
            m += mass
        print(f"mass all = {m}")
        break
    mass_ratio = input("mass ratio : ")
    print(float(motor_mass) * float(motor_n) + float(mass_base) * float(mass_ratio))
    mass_set = input("what you set : ")
    masses.append(mass_set)
    print("\n")
