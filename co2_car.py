import matplotlib.pyplot as plt

# Define initial time, position, velocity, and acceleration
time = 0.
position = 0.
velocity = 0.
acceleration = 0.

# Propulsive force from CO2 canister, and its duration
Fpropulsive = 17.39  # Newtons
propulsion_duration = 0.7

# Drag coefficient
Cd = 1.2

# Friction coefficient
friction_coeff = 0.1

# Car mass, in kilograms
mass = 2.78/1000

# Density of air, kg/m^3
density = 1.2

# Frontal area of car, in m^2
Afrontal = 0.

# Time step size, in s
deltat = 0.005

# Initialize lists, for plotting
timelist = []
velocitylist = []
positionlist = []

# Step through time to simulate car motion
while velocity > -0.1:

	Fdrag = Cd*0.5*density*(velocity*velocity)*Afrontal
	Ffriction = friction_coeff*mass*9.81
	
	# Calculate the net force, accounting for whether or not the canister is firing
	if time < propulsion_duration:
		net_force = Fpropulsive - Fdrag - Ffriction
	else:
		net_force = -Fdrag - Ffriction
	#--------------------------------------
		
	acceleration = net_force/mass
	
	velocity = velocity + acceleration*deltat
	
	position = position + velocity*deltat
	
	time = time + deltat
	
	timelist = timelist + [time]
	velocitylist = velocitylist + [velocity]
	positionlist = positionlist + [position]
	
	print("Time= "+str(time)+" Position="+str(position))

# Plot everything
plt.plot(timelist,positionlist)
plt.plot(timelist,velocitylist)
plt.show()
