#include <cstddef>
#include <cstring>
const char *xml_header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
const char *sample = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<a>aaa</a>";

#include <libxml/parser.h>
#include <libxml/tree.h>

int main(void)
{
        xmlDocPtr xml;
        xml = xmlReadMemory(sample, strlen(sample), "random.xml", NULL, 0);

        xmlFreeDoc(xml);

}