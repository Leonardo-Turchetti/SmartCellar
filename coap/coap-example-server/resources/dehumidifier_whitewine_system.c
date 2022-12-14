#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "coap-engine.h"
#include "dev/leds.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "dehumidifier_whitewine-system"
#define LOG_LEVEL LOG_LEVEL_APP

static void dehumidifier_whitewine_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dehumidifier_whitewine_system,
         "title=\"Dehumidifier White Wine System\";rt=\"Control\"",
         NULL,
         NULL,
         dehumidifier_whitewine_put_handler,
         NULL);

bool dehumidifier_whitewine_on = false;

static void dehumidifier_whitewine_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
	size_t len = 0;
	const char *text = NULL;
	char mode[4];
	memset(mode, 0, 3);

	int mode_success = 1;

	len = coap_get_post_variable(request, "mode", &text);
	if(len > 0 && len < 4) {
		memcpy(mode, text, len);
		if(strncmp(mode, "ON", len) == 0) {
			dehumidifier_whitewine_on = true;
			leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
			LOG_INFO("Dehumidifier White Wine System ON\n");
		} else if(strncmp(mode, "OFF", len) == 0) {
			dehumidifier_whitewine_on = false;
			leds_set(LEDS_NUM_TO_MASK(LEDS_RED));
			LOG_INFO("Dehumidifier White Wine System OFF\n");
		} else {
			mode_success = 0;
		}
	} else {
		mode_success = 0;
	}
	
	if(!mode_success) {
    		coap_set_status_code(response, BAD_REQUEST_4_00);
 	}
}