import os
import sys
import random

import io_utils
import bash_utils
import trim_simulator_utils

def help(terminate = True):
	io_utils.usage("python ssd_event_generator -r <duration(second)>")
	io_utils.usage("python ssd_event_generator --real <duration(second)>")
	io_utils.usage("python ssd_event_generator -s <duration(second)> <real time> <write time> <trim time>")
	io_utils.usage("ython ssd_event_generator --simulate <duration(second)> <real time> <write time> <trim time>", terminate = terminate)

class SSD_Event_Generator(object):
	def __init__(self, real_event_capture, simulate_event_capture, capture_duration, read_time, write_time, trim_time):
		self.real_event_capture = real_event_capture
		self.simulate_event_capture = simulate_event_capture
		self.capture_duration = capture_duration
		self.read_time = read_time
		self.write_time = write_time
		self.trim_time = trim_time


	def capture_real_event(self):
		# container for list of events
		events = []
		cmd = "sudo blktrace -a read -a write -a discard -w %d -d /dev/sda -o - | blkparse -i -"%(self.capture_duration)
		execute, message = execute_bash_command(cmd, self.capture_duration + 5)
		if not execute:
			io_utils.stderr("Unable to capture real even, try simulate mode", terminate = True)	

		print message
		events = message.split("\n")
		return events

	def capture_simulate_event(self):
		# 8,0    1       70     0.000039646   213  D  WS 415398104 + 176 [jbd2/sda5-8]
		# 8,0    1       71     0.000288625     0  C  WS 415398104 + 176 [0]
		CLOCK_SPEED = 0.000000001
		clock = 0
		events = {}
		seq_number = 0
		event_id = random.randint(100000000, 999999999)
		while(clock < self.capture_duration):
			event_type = random.randint(0, 1) 
			# no event
			if event_type == 0:
				pass
			# issue D
			elif event_type == 1
				rwbs = random.randint(0, 2)
				event_id = 
				# read
				if rwbs == 0:
					events[clock] = "8,0    1       %s     %s   213  D  R %s"%(seq_number, clock, event_id)
					events[clock] = "8,0    1       %s     %s   213  C  R %s"%(seq_number + 1, clock + self.read_time, event_id)
					seq_number += 2
					event_id += 1
				# write
				elif rwbs == 1:
					pass
				# trim
				else:
					pass

			clock += CLOCK_SPEED

		return [events[key] for key in sorted(events.keys())]

	def capture_event(self):
		if self.real_event_capture and capture_duration > 0:
			return self.capture_real_event()
		elif self.simulate_event_capture and capture_duration > 0 and read_time > 0 and write_time > 0 and trim_time > 0:
			return self.capture_simulate_event()
		else:
			help()



if __name__ == '__main__':
	argc = len(sys.argv)
	if argc < 3:
		help()

	real_event_capture = False
	simulate_event_capture = False
	capture_duration = -1
	read_time = -1
	write_time = -1
	trim_time = -1

	# prase options
	index = 1
	while(index < argc):
		arg = sys.argv[index]
		print(arg)
		if arg == "-r" or arg == "--real":
			real_event_capture = True
			try:
				capture_duration = int(sys.argv[index + 1])
				index += 1
			except IndexError:
				io_utils.stderr("Dangling -r or --real flag on command line", terminate = True)
		elif arg == "-s" or arg == "--simulate":	
			simulate_event_capture = True
			try:
				capture_duration = int(sys.argv[index + 1])
				read_time = int(sys.argv[index + 2])
				write_time = int(sys.argv[index + 3])
				trim_time = int(sys.argv[index + 4])
				index += 4
			except IndexError:
				io_utils.stderr("Dangling -s or --simulate flag on command line", terminate = True)
		else:
			io_utils.stderr("Unrecognized option - %s"%arg, terminate = True)

		index += 1

	# create generator instance
	event_generator = SSD_Event_Generator(real_event_capture, simulate_event_capture, capture_duration, read_time, write_time, trim_time)
	event_generator.capture_event()

