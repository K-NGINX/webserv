#!/usr/bin/env python3
import os
import cgi
import cgitb
from PIL import Image
from io import BytesIO

cgitb.enable()  # for troubleshooting

save_path = os.environ['SAVE_PATH']

# read multipart/form-data body from pipe
form = cgi.FieldStorage()

fileitem = form['file']

try:
    if fileitem.filename:
        im = Image.open(BytesIO(fileitem.file.read()))
        im.save(os.path.join(save_path, fileitem.filename))

        # success
        print("HTTP/1.1 303 See Other", end="\r\n")
        print("Location: /album", end="\r\n")
        print(end="\r\n")
    else:
        # failure
        print("HTTP/1.1 500 Internal Server Error", end="\r\n")
        print("Location: /errors/5xx.html", end="\r\n")
        print(end="\r\n")
        
except Exception as e:
    print("HTTP/1.1 500 Internal Server Error", end="\r\n")
    print("Location: /errors/5xx.html", end="\r\n")
    print(end="\r\n")