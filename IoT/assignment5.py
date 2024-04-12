import math

def free_space_path_loss(distance):
    frequency = 2.4 * 10**9  # Frequency in Hz
    speed_of_light = 3 * 10**8  # Speed of light in m/s
    wavelength = speed_of_light / frequency
    path_loss = (4 * math.pi * distance / wavelength)**2
    return 20 * math.log10(path_loss)

def shadowing_path_loss(distance, standard_deviation):
    free_space_loss = free_space_path_loss(distance)
    shadowing_loss = standard_deviation * random.gauss(0, 1)
    path_loss = free_space_loss + shadowing_loss
    return path_loss

# Example usage
distance = 1000  # Distance between Tx and Rx in meters
standard_deviation = 4  # Standard deviation for shadowing effect
path_loss_free_space = free_space_path_loss(distance)
path_loss_shadowing = shadowing_path_loss(distance, standard_deviation)

print("Path Loss (Free Space):", path_loss_free_space, "dB")
print("Path Loss (Shadowing):", path_loss_shadowing, "dB")