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
<html>\n\
    <head>\n\
        <title>\n\
            File share\n\
        </title>\n\
        <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>\n\
        <!-- <script src=\"http://cdn.socket.io/stable/socket.io.js\"></script> -->\n\
        <script>\n\
            $(document).ready(function(){\n\
                if (\"WebSocket\" in window) {\n\
                    alert(\"WebSocket is supported by your Browser!\");\n\
                }else{\n\
                    alert(\"WebSocket is not supported by your Browser!\");\n\
                }\n\
                var ws = new WebSocket(\"ws://localhost:8000/\");\n\
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
                       var file = document.getElementById('#pfile');\n\
                    //    try {\n\
                    //     var ws = new Websocket(document.URL);\n\
                    //    } catch (error) {\n\
                    //        console.log(error);\n\
                    //    }\n\
                       console.log(\"Socket connected successfully\");\n\
                       ws.onopen = function(event){\n\
                            console.log(\"Socket onopen successfully\");\n\
                           try{\n\
                               ws.send(file.name);\n\
                               ws.send(file.size);\n\
                           } catch(exception){\n\
                            $(\"disp\").text(\"Exception: \"+ exception);\n\
                            ws.close(); return false;\n\
                           }\n\
                           reader.readAsArrayBuffer(file);\n\
                           try{\n\
                               ws.send(arrbuf);\n\
                           } catch(exception){\n\
                            $(\"disp\").text(\"Exception: \"+ exception);\n\
                            ws.close(); return false;\n\
                           }\n\
                           try {\n\
                               ws.onmessage = function(event){\n\
                                   buf = event.data;\n\
                               }\n\
                           } catch (exception) {\n\
                            $(\"disp\").text(\"Exception: \"+ exception);\n\
                            ws.close(); return false;\n\
                           }\n\
                           if(buf){\n\
                            $(\"disp\").text(\"Saved\");   \n\
                           }\n\
                           else{\n\
                            $(\"disp\").text(\"Failed to save\");\n\
                           }\n\
                       }\n\
                    //    ws.close();\n\
                   });\n\
                   $('#download').click(function(){\n\
                       var buf = \"get \";\n\
                       var fname = $('#gfile').val();\n\
                       buf += fname;\n\
                       var ws = new Websocket(document.URL);\n\
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
            });\n\
        </script>\n\
        <style>\n\
            #pfile, #upload, #gfile, #download{\n\
                display: none;\n\
            }\n\
        </style>\n\
    </head>\n\
    <body>\n\
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
    </table>\n\
    <div></div>\n\
    </body>\n\
</html>";

// void encode(std::string& data);
// void escape1(std::string *data);

void html(int soc, char buf[1024], int valend){
    printf("\nhello from html\n");
    send(soc, server_message1, strlen(server_message1), 0);
}

string encode(string input){
    // string response = SHA1(append(base64_decode(key64), "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
    // string response_encoded = base64_encode(response);
    // return response_encoded;

}