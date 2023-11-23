#!/usr/bin/env python3
import os
import cgi
import sys

relative_path = "/photos/"
absolute_path = os.environ['SAVE_PATH'] + relative_path

if not os.path.exists(absolute_path):
    os.makedirs(absolute_path)

png_files = [f for f in os.listdir(absolute_path) if f.lower().endswith((".png", ".PNG"))]


album_html = """
<!DOCTYPE html>
<html>
<head>
    <title>Show Album</title>
    <style>
        body {
            background-color: white;
            font-family: Trebuchet MS;
        }
        h1 {
            margin: 72px 0;
			font-size: 48px;
            text-align: center;
            color: #666666
        }
        .home-button {
            position: absolute;
            top: 20px;
            left: 20px;
            color: #666666;
            text-decoration: none;
        }
        .home-button:visited {
            color: #666666;
        }
    </style>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/sweetalert2@11"></script>
</head>
<body>
    <a href="/" class="home-button">Home</a>
    <h1>Album</h1>
"""

for file in png_files:
    album_html += "<img src='{}' alt='Image' height='250'>".format(os.path.join(relative_path, file))
    album_html += "<button class='delete-btn' data-filepath='{}'>Delete</button>".format(os.path.join(relative_path, file))

album_html += """
    <script>
        $(document).ready(function(){
            $('.delete-btn').on('click', function(e){
                e.preventDefault();
                var filePath = $(this).data('filepath');
                $.ajax({
                    url: filePath,
                    type: 'DELETE',
                    success: function(result) {
                        // 요청 성공
                        Swal.fire({
                        title: 'Deleted!',
                        text: 'Your file has been deleted.',
                        icon: 'success',
                        confirmButtonText: 'OK'
                        }).then((result) => {
                            // OK 버튼이 눌렸을 때
                            if (result.isConfirmed) {
                                location.reload();
                            }
                        })
                    }
                });
            });
        });
    </script>
</body>
</html>
"""

print("HTTP/1.1 200 OK", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print(end="\r\n")
print(album_html)
