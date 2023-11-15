#!/usr/bin/env python3
import os
import cgi
import cgitb
import base64
from pathlib import Path

cgitb.enable()

# 저장할 위치
save_path = os.environ.get('SAVE_PATH')

# 표준 입력에서 데이터 읽기
form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ, keep_blank_values=True)

# multipart/form-data에서 png 데이터와 파일 이름 추출
fileitem = form['file'] # 'file'은 multipart/form-data의 필드명입니다.
filename = fileitem.filename
png_data = fileitem.file.read()

# 파일로 저장
with open(Path(save_path) / filename, 'wb') as f:
    f.write(png_data)
