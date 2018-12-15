"""
This program is used to generate random input to the billiards simulation
software. Values were based in real rules and specifications retrieved from
the links below.

https://www.quora.com/What-is-the-difference-between-billiards-pool-and-snooker-1
https://wpapool.com/equipment-specifications/#Design
http://files.umb-carom.org/Public/Rules/Carom_Rules.pdf
https://www.wcbs-billiards.org/
http://billiards.colostate.edu/threads/speed.html
http://billiards.colostate.edu/threads/physics.html
"""


from random import choice, randint, uniform
from sys import argv

from numpy import linspace

possible_ball_speed = [-6,-5,-4,-3,-2,-1,1,2,3,4,5,6]

possible_ball_pos = []

# Billiards balls diameter ranges from ~2 to ~2.42 inches (~5 to ~6.2 centimeters)
radius = [randint(3, 3) for i in range(int(argv[1]))]

min_distance = max(radius)
padding = 2
friction = 0.001
e_wall = 0
e_ball = 0

def generate_possible_positions(table_w, table_l):
	global possible_ball_pos

	i = min_distance + padding
	j = min_distance + padding

	while i < table_w:
		j = min_distance + padding
		while j < table_l:
			possible_ball_pos.append([i, j])
			j += min_distance * 2
		i += min_distance * 2

def random_table():
	w = randint(100, 700)

	return 3000, 3000

def random_ball(table_w, table_l, radius, i):
	"""Generate random ball parameters"""

	# Billiards balls mass ranges from ~6 to ~7.5 oz (~0.17 to ~0.22 kilograms)
	mass = randint(1, 6)

	# Initial position
	try:
		init_pos = choice(possible_ball_pos)
		possible_ball_pos.remove(init_pos)
	except IndexError as e:
		print(f"Could only create {i} balls!")
		return None

	# Initial speed
	speed = [
		choice(possible_ball_speed),
		choice(possible_ball_speed)
	]

	return f"{i} {radius} {mass} {init_pos[0]} {init_pos[1]} {speed[0]} {speed[1]}\n"

if __name__ == '__main__':
	import sys

	mode = sys.argv[1]
	w, l = random_table()
	generate_possible_positions(w, l)

	with open(sys.argv[2], "w") as f:
		f.write(f"{w} {l} {argv[1]}\n")
		f.write(f"{friction} {e_wall} {e_ball}\n")

		for i in range(int(argv[1])):
			ball = random_ball(w, l, radius[i], i)
			if ball is not None:
				f.write(ball)
			else:
				break
