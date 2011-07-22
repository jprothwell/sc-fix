#include "host_config.h"
#include "host_types.h"

#include "papi.h"
#include "goep_obex_object.h"
#include "goep_pdu_encoder.h"

int main(void)
{
  struct prh_obex_object* pObject;

  u_int8  pHeader[20];

  pObject = prh_obex_create_object();

  prh_obex_add_header(pObject,pHeader);

  prh_obex_delete_object(pObject);

  return 1;
}




















