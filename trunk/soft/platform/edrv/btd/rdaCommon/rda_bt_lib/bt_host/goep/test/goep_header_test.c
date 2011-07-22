#include "host_config.h"
#include "host_types.h"

#include "goep_header_encoder.h"

int main(void)
{
  u_int8 id;
  u_int16 len;
  u_int8* pData;
  u_int32 data_long;
  u_int8 data_small;

  prh_obex_create_string_header(id,len,pData);

  prh_obex_create_sequence_header(id,len,pData);
  
  prh_obex_create_byte_header(id, data_small);
  
  prh_obex_create_int_header(id, data_long);
  

  return 1;
}
