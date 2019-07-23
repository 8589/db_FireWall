#include "logger.h"


int main(){
	logger log;
	log.debug("deug");
	log.warning("warning");
	log.error("error");
	return 0;
}