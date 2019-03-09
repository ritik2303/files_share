#include "html.h"
#include "wsHandshake.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

string handleRequest(string request);

char *index_message1 =
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

void html_req(int soc, char buf[1024], int valend){
    printf("\nhello for html terminal\n");
    // send(soc, server_message2, strlen(server_message2), 0);

    stringstream requestStream;
    printf("valend %d\n",valend);
    
    if (valend <= 0) {
        cout << "Errno: " << errno << "\n";
    }
    requestStream.write(buf, valend);
        // cout << "Client " << i << ": Status: " << valend << "\n";
    cout << "Data: " << requestStream.str() << "\n";

    string asd = handleRequest(requestStream.str());


    send(soc, asd.c_str(), strlen(asd.c_str()), 0);



}

string handleRequest(string request){
    cout<< request << endl;
    if (request[0] == 'G' &&
        request[1] == 'E' &&
        request[2] == 'T') {
        
        // handshake!
        printf("handshake");
        
        wsHandshake handshake(request);
        handshake.generateResponse();
        
        if (!handshake.isSuccess()) {
            printf("handshake failed");
            return "";
        }

        return handshake.getResponse();
    }
    return "";
}