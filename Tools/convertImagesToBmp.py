# this script requires PIL
# to install PIL, execute the following command line:
#  python -m pip install PIL
import sys
import os
from PIL import Image
files = []
validExtensionList = ['.png', '.jpg', '.tga']
print(len(sys.argv))
if len(sys.argv) != 2:
    print("Incorrect Syntax!")
    print("    Syntax: convertImagesToBmp.py <full path to folder>")
    print("    Example: convertImagesToBmp.py C:\data\MyImages")
else:
    # retrieve input path
    path = sys.argv[1]
    # r=root, d=directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            for extensionStr in validExtensionList:
                if extensionStr in file:
                    files.append(os.path.join(r, file))
    print("files " + str(len(files)))
	#print(files)
    for imageFile in files:
        try:
            print(imageFile)
            file_in = imageFile
            img = Image.open(file_in)
            file_out, file_extension = os.path.splitext(file_in)
            file_out = file_out + '.bmp'
            img.save(file_out)
        except:
            print("Failed to convert!")