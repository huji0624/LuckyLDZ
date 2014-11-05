#!/usr/bin/python

import sys
import os

if len(sys.argv)==1:
	print "python imageMaker.py config sourceimage [--ignorename]"
	exit(1)

confpath = sys.argv[1]
source = sys.argv[2]

f = file(confpath,'r')
text = f.read()
f.close()

lines = text.strip().split('\n')
for l in lines:
	confs = l.strip().split(' ')
	outname = confs[0].strip()
	width = confs[1].strip()
	height =confs[2].strip()
	if '--ignorename' in sys.argv:
		basename = os.path.basename(source)
		prename = basename.split('.')[0]
		command = 'sips -z %s %s %s --out %s_%s_%sx%s.png' % (height,width,source,prename,outname,width,height)
	else:
		command = 'sips -z %s %s %s --out %s' % (height,width,source,outname)
	rt = os.system(command)
