import math

print "static const uint8_t table[256] = {"
for x in range (0, 255):
	print str(int(128 + (127 * math.sin(math.radians(x * (360.0/256)))))) + ","
