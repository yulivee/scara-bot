# Open a file
fo = open("foo.txt", "r+")
str = fo.read(10)
print ("Read String is : ", str)

# Check current position
position = fo.tell()
print ("Current file position : ", position)

# Reposition pointer at the beginning once again
position = fo.seek(0, 0)
str = fo.read()
print ("Again read String is : ", str)
# Check current position
position = fo.tell()
print ("Current file position : ", position)

#Read the First two Lines
position = fo.seek(0, 0) # Reposition pointer at the beginning once again
print ("First Line: " + fo.readline().rstrip('\n'))
print ("Second Line: " + fo.readline().rstrip('\n'))
# Check current position
position = fo.tell()
print ("Current file position : ", position)

#'tag' File
fo.write( "\nPython was here!")

# Close opened file
fo.close()