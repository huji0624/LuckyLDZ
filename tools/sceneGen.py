#!/usr/bin/python

import sys

name = sys.argv[1]
upname = name.upper()

print "//------.h"
print "#ifndef __%s__" % (upname)
print "#define __%s__" % (upname)
print "#include \"cocos2d.h\""
print "class %s : public cocos2d::LayerColor" % (name)
print "{"
print "public:"
print "	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);"
print "	bool initDict(cocos2d::CCDictionary *dic);"
print "	virtual ~%s();" % (name)
print "	CREATE_FUNC(%s);" % (name)
print "};"
print "#endif"

print "//------.cpp"
print "#include \"%s.h\"" % (name)
print "#include \"LHMacros.h\""
print "USING_NS_CC;"
print "Scene* %s::createScene(cocos2d::CCDictionary *dic)" % (name)
print "{"
print "	auto scene = Scene::create();"
print "	auto layer = %s::create();" % (name)
print "	layer->initWithColor(Color4B::WHITE);"
print "	layer->initDict(dic);"
print "	scene->addChild(layer);"
print "	return scene;"
print "}\n"
print "%s::~%s(){" % (name,name)
print "}"
print "bool %s::initDict(cocos2d::CCDictionary *dic)" % (name)
print "{\n"
print "\treturn true;\n}"
