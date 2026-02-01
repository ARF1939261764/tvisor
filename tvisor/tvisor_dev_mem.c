#include "tvisor.h"
#include "tvisor_dev.h"

int tvisor_dev_mem_init(void *ctx){
  return TVISOR_STATUS_OK;
}

tvisor_dev_ops_t tvisor_dev_mem = {
  .init = tvisor_dev_mem_init
};