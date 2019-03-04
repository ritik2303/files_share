#include "html.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

char *server_message1 =
"HTTP/1.1 200 OK \n\
\n\
<html>\n\
<head>\n\
  <title>My first styled page</title>\n\
</head>\n\
\n\
<body>\n\
\n\
<!-- Main content -->\n\
<h1>My first styled page</h1>\n\
\n\
<p>Welcome to my styled page!\n\
\n\
<p>It lacks images, but at least it has style.\n\
And it has links, even if they don't go\n\
anywhere&hellip;\n\
\n\
<p>There should be more here, but I don't know\n\
what yet.\n\
\n\
<!-- Sign and date the page, it's only polite! -->\n\
<address>Made 5 April 2004<br>\n\
  by myself.</address>\n\
\n\
</body>\n\
</html>";
// void encode(std::string& data);
// void escape1(std::string *data);

void html(int soc){
    printf("\nhello from html\n");
    send(soc, server_message1, strlen(server_message1), 0);
}

// void encode(std::string& data) {
//     std::string buffer;
//     buffer.reserve(data.size());
//     for(size_t pos = 0; pos != data.size(); ++pos) {
//         switch(data[pos]) {
//             case '&':  buffer.append("&amp;");       break;
//             case '\"': buffer.append("&quot;");      break;
//             case '\'': buffer.append("&apos;");      break;
//             case '<':  buffer.append("&lt;");        break;
//             case '>':  buffer.append("&gt;");        break;
//             default:   buffer.append(&data[pos], 1); break;
//         }
//     }
//     data.swap(buffer);
// }

// void escape1(std::string *data)
// {
//     using boost::algorithm::replace_all;
//     replace_all(*data, "&",  "&amp;");
//     replace_all(*data, "\"", "&quot;");
//     replace_all(*data, "\'", "&apos;");
//     replace_all(*data, "<",  "&lt;");
//     replace_all(*data, ">",  "&gt;");
// }