#!/usr/bin/python3

import sys
from os import system

if __name__ == "__main__":
	if (len(sys.argv) < 2):
		print ("\t|--ERROR Enter filename")
		exit()

	filename = sys.argv[1]

	print ("\t|--STATE Creating code file")

	fd = open(filename, 'r')
	lines = fd.readlines()
	fd.close()

	n = 0
	output = ''

	for line in lines:
		vars = line.split(' ')
		size = len(vars)

		output += 'constraint '
		for i in range(size - 2):
			num = int(vars[i], 10)
			if (n < num):
				n = num

			output += 'x[%d] xor ' % (num)

		num = int(vars[size - 2], 10)
		if (n < num):
			n = num

		output += 'x[%d] == 0;\n' % (num)

	mzncode = '''
int: n = %d;
set of int: size = 1..n;

array[size] of var bool: x;
var 1..n: total = sum(i in size)(bool2int(x[i]));

%s
constraint total != 0;

solve minimize total;

output ["total = \(total)\\nTrue variables: "];
output [ "\(i) " | i in size where fix(x[i] == true) ];
''' % (n, output)

	fd = open('solve.mzn', 'w')
	fd.write(mzncode)
	fd.close()

	print ("\t|--STATE Executing minizinc solver")

	system('minizinc --solver Gecode solve.mzn')

	print ("\t|--STATE End of program")
