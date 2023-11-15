#!/usr/bin/env python3
import os
import cgi
import cgitb
from PIL import Image
from io import BytesIO

cgitb.enable()  # for troubleshooting

# 환경변수에서 필요한 정보를 얻어옵니다.
request_method = os.environ['REQUEST_METHOD']
content_type = os.environ['CONTENT_TYPE']
content_length = os.environ['CONTENT_LENGTH']
save_path = os.environ['SAVE_PATH']

# multipart/form-data의 본문을 파이프로부터 읽어옵니다.
form = cgi.FieldStorage()
# 본문에서 파일 데이터를 불러옵니다.
fileitem = form['file']

if fileitem.filename:
    # BytesIO를 이용해 바이너리 데이터를 PIL.Image로 읽어들입니다.
    im = Image.open(BytesIO(fileitem.file.read()))

    # 이미지를 PNG 형식으로 저장합니다.
    im.save(os.path.join(save_path, fileitem.filename))

    # 파일 저장에 성공하였음을 알립니다.
    print("Success: The file was saved as {}.".format(fileitem.filename))
else:
    # 파일 저장에 실패하였음을 알립니다.
    print("Failed: No file was uploaded.")
