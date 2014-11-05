#!/usr/bin/python

def printPath(root):
	files = os.listdir(root)
	for f in files:
		path = root+"/"+f
		if os.path.isdir(path):
			printPath(path)
		else:
			if path.endswith(".c") or path.endswith(".cpp"):
				print path.replace("tools","..") + " \\"

import sys
import os

rootpath = os.path.dirname(__file__) + "/../Classes"

printPath(rootpath)