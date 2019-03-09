#include "html.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

char *shakehand = 
"";

char *server_message1 =
"HTTP/1.1 200 OK \n\
\n\
<!DOCTYPE html>\n\
<html>\n\
<head>\n\
<style>\n\
html, body {\n\
  width: 100%;  \n\
  height: 100%;\n\
  background: #FF7777;\n\
  -webkit-font-smoothing: antialiased;\n\
  display: flex;\n\
  justify-content: center;\n\
  align-items: center;\n\
}\n\
\n\
h1 {\n\
  height: 100px;\n\
}\n\
\n\
h1 span {\n\
  position: relative;\n\
  top: 20px;\n\
  display: inline-block;\n\
  animation: bounce .3s ease infinite alternate;\n\
  font-family: 'Titan One', cursive;\n\
  font-size: 80px;\n\
  color: #FFF;\n\
  text-shadow: 0 1px 0 #CCC,\n\
               0 2px 0 #CCC,\n\
               0 3px 0 #CCC,\n\
               0 4px 0 #CCC,\n\
               0 5px 0 #CCC,\n\
               0 6px 0 transparent,\n\
               0 7px 0 transparent,\n\
               0 8px 0 transparent,\n\
               0 9px 0 transparent,\n\
               0 10px 10px rgba(0, 0, 0, .4);\n\
}\n\
\n\
h1 span:nth-child(2) { animation-delay: .1s; }\n\
h1 span:nth-child(3) { animation-delay: .2s; }\n\
h1 span:nth-child(4) { animation-delay: .3s; }\n\
h1 span:nth-child(5) { animation-delay: .4s; }\n\
h1 span:nth-child(6) { animation-delay: .5s; }\n\
h1 span:nth-child(7) { animation-delay: .6s; }\n\
h1 span:nth-child(8) { animation-delay: .7s; }\n\
\n\
@keyframes bounce {\n\
  100% {\n\
    top: -20px;\n\
    text-shadow: 0 1px 0 #CCC,\n\
                 0 2px 0 #CCC,\n\
                 0 3px 0 #CCC,\n\
                 0 4px 0 #CCC,\n\
                 0 5px 0 #CCC,\n\
                 0 6px 0 #CCC,\n\
                 0 7px 0 #CCC,\n\
                 0 8px 0 #CCC,\n\
                 0 9px 0 #CCC,\n\
                 0 50px 25px rgba(0, 0, 0, .2);\n\
  }\n\
}\n\
  button {\n\
    display: inline-block;\n\
    font-size: 24px;\n\
    cursor: pointer;\n\
    text-align: center;\n\
    text-decoration: none;\n\
    outline: none;\n\
    color: #fff;\n\
    background-color: #4CAF50;\n\
    border: none;\n\
    border-radius: 3px;\n\
    box-shadow: 0 2px #999;\n\
  }\n\
  \n\
  button:hover {background-color: #3e8e41}\n\
  \n\
  button:active {\n\
    background-color: #3e8e41;\n\
    box-shadow: 0 1px #666;\n\
    transform: translateY(4px);\n\
  }\n\
\n\
    h1 {\n\
        display: none;\n\
    }\n\
\n\
    table {\n\
        width:500px;\n\
        height:150px;\n\
        background: rgb(241, 85, 85);\n\
        position:fixed;\n\
        margin-left:-150px; /* half of width */\n\
        margin-top:-150px;  /* half of height */\n\
        top:50%;\n\
        left:50%;\n\
    }\n\
\n\
#pfile, #upload, #gfile, #download{\n\
    display: none;\n\
}\n\
<!-- #pfile, #upload, #gfile, #download, #get, #put{ -->\n\
\n\
</style>\n\
\n\
<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>\n\
        <!-- <script src=\"http://cdn.socket.io/stable/socket.io.js\"></script> -->\n\
    <script>\n\
        $(document).ready(function(){\n\
            var ws = new WebSocket(\"ws://localhost:8000/\");\n\
            console.log(\"Socket connected successfully\");\n\
            \n\
            var reader = new FileReader();\n\
            reader.onload = function(e){\n\
                var arrbuf = reader.result;\n\
            }\n\
            $(\"#put\").click(function(){\n\
                $('#pfile').css(\"display\",\"block\");\n\
                $('#upload').css(\"display\",\"block\");\n\
                $('#gfile').css(\"display\",\"none\");\n\
                $('#download').css(\"display\",\"none\");\n\
            });\n\
            $(\"#get\").click(function(){\n\
                $('#pfile').css(\"display\",\"none\");\n\
                $('#upload').css(\"display\",\"none\");\n\
                $('#gfile').css(\"display\",\"block\");\n\
                $('#download').css(\"display\",\"block\");\n\
            });\n\
            $('#upload').click(function(){\n\
                var buf = \"put \";\n\
                var file = document.getElementById('#pfile').files;\n\
                    console.log(\"Socket onopen successfully\");\n\
                    try{\n\
                        ws.send(file[0].name);\n\
                        ws.send(file[0].size);\n\
                    } catch(exception){\n\
                    $(\"#disp\").text(\"Exception: \"+ exception);\n\
                    ws.close(); return false;\n\
                    }\n\
                    reader.readAsArrayBuffer(file[0]);\n\
                    try{\n\
                        ws.send(arrbuf);\n\
                    } catch(exception){\n\
                    $(\"#disp\").text(\"Exception: \"+ exception);\n\
                    ws.close(); return false;\n\
                    }\n\
                    try {\n\
                        ws.onmessage = function(event){\n\
                            buf = event.data;\n\
                        }\n\
                    } catch (exception) {\n\
                    $(\"#disp\").text(\"Exception: \"+ exception);\n\
                    ws.close(); return false;\n\
                    }\n\
                    if(buf){\n\
                    $(\"#disp\").text(\"Saved\");   \n\
                    }\n\
                    else{\n\
                    $(\"#disp\").text(\"Failed to save\");\n\
                    }\n\
            });\n\
            $('#download').click(function(){\n\
                var buf = \"get \";\n\
                var fname = $('#gfile').val();\n\
                buf += fname;\n\
                ws.onopen = function(event){\n\
                    try {\n\
                    ws.send(buf);\n\
                    } catch (exception) {\n\
                    $(\"#disp\").text(\"Exception: \" + exception+\"\\nException sending data\");\n\
                    ws.close(); return false;\n\
                    }\n\
                    try {\n\
                        ws.onmessage = function(event){\n\
                            buf = event.data;\n\
                        }\n\
                    } catch (exception) {\n\
                    $(\"#disp\").text(\"Exception: \" + exception+\"\\nException recieving data\");\n\
                    ws.close(); return false;\n\
                    }\n\
                }\n\
                feed = new File(buf, fname);\n\
                $(\"#download\").click(function(){\n\
                uriContent = \"data:application/octet-stream,\" + encodeURIComponent(content);\n\
                newWindow = window.open(uriContent, 'feed');\n\
                });\n\
            });\n\
            $('#list').click(function(){\n\
                var buf = \"ls\";\n\
                    try {\n\
                        ws.send(buf);\n\
                    } catch (error) {\n\
                        $(\"#disp\").text(\"Exception: \" + exception+\"\\nException sending data\");\n\
                        ws.close(); return false;\n\
                    }\n\
                    try {\n\
                        ws.onmessage = function(event){\n\
                            buf = event.data;\n\
                        }\n\
                    } catch (error) {\n\
                        $(\"#disp\").text(\"Exception: \" + exception+\"\\nException recieving data\");\n\
                        ws.close(); return false;\n\
                    }\n\
            });\n\
        });\n\
    </script>\n\
</head>\n\
<body>\n\
\n\
    <h1>\n\
        <span>F</span>\n\
        <span> </span>\n\
        <span>C</span>\n\
        <span>l</span>\n\
        <span>o</span>\n\
        <span>u</span>\n\
        <span>d</span>\n\
        <span>!</span>\n\
    </h1>\n\
\n\
    <!-- <form>\n\
        <button class=\"button\" style=\"vertical-align:middle\" formaction=\"all/\"><span>Go</span></button>\n\
    </form> -->\n\
\n\
    <table>\n\
        <tr>\n\
            <td><button id=\"put\">PUT</button></td>\n\
            <td><input type=\"file\" id=\"pfile\"></td>\n\
            <td><button id=\"upload\">Upload</button></td>\n\
        </tr>\n\
        <tr>\n\
            <td><button id=\"get\">GET</button></td>\n\
            <td><input type=\"text\" id=\"gfile\"></td>\n\
            <td><button id=\"download\">Download</button></td>\n\
        </tr>\n\
        <tr>\n\
            <td colspan=\"3\" style = \"text-align: center;\"><button id=\"list\">List all</button></td>\n\
        </tr>\n\
        \n\
        <tr>\n\
            <td colspan=\"3\"><div id=\"disp\"></div></td>\n\
        </tr>\n\
\n\
    </table>\n\
    \n\
\n\
</body>\n\
</html>";

void html(int soc, char buf[1024], int valend){
    printf("\nhello from html\n");
    send(soc, server_message1, strlen(server_message1), 0);
}