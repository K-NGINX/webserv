#!/usr/bin/env python3
import os
import cgi
import sys

photo_dir = os.environ['SAVE_PATH'] + "/photos/"
web_photo_dir = "/photos/"

png_files = [f for f in os.listdir(photo_dir) if f.endswith(".png")]
album_html = ""

album_html += "<html>\n"
album_html += "<head>\n"
album_html += "<title>Photo Gallery</title>\n"
album_html += "</head>\n"
album_html += "<body>\n"

for png_file in png_files:
    album_html += "<img src='{}' width='200'><br>\n".format(web_photo_dir + png_file)
    album_html += "<form method='POST' action='지우는CGI.py'>\n"
    album_html += "<input type='hidden' name='filename' value='{}'>\n".format(png_file)
    album_html += "<input type='submit' value='Delete'>\n"
    album_html += "</form>\n"
    album_html += "<br>\n"

album_html += "</body>\n"
album_html += "</html>"

print("HTTP/1.1 200 OK", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print()
print(album_html)
