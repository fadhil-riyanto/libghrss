#include "libxml/xmlmemory.h"
#include "libxml/xmlstring.h"
#include <cstddef>
#include <cstring>
const char *xml_header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
const char *sample = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<a>aaa</a>";

#include <libxml/parser.h>
#include <libxml/tree.h>

static void _internal_parse_entry(xmlDocPtr xml, xmlNodePtr cur)
{
        xmlChar *id;
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar*)"id")) {
                        id = xmlNodeListGetString(xml, cur->xmlChildrenNode, 1);
                        printf("%s\n", id);
                        xmlFree(id);
                }
                cur = cur->next;
        }
}

int main(void)
{
        xmlDocPtr xml;
        xml = xmlReadFile("releases.atom", NULL, 0);

        xmlNodePtr cur;
        cur = xmlDocGetRootElement(xml);

        if (xmlStrcmp(cur->name, (const xmlChar*)"feed")) {
                printf("xml root invalid");
                goto free;
        }

        cur = cur->xmlChildrenNode;
        while(cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar*)"entry")) {
                        _internal_parse_entry(xml, cur);
                }
                
                cur = cur->next;

        }
        
free:
        xmlFreeDoc(xml);

}