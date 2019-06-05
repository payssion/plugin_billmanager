#include <billmgr/payment/paymentmodule.h>

using namespace payment;

#define BINARY_NAME "pmpayssion"

class Payssion : public Module {
public:
	Payssion() : Module("payssion", "1.0")
	{
		feature_map[PAYMENT_FEATURE_REDIRECT] = true;
		feature_map[PAYMENT_FEATURE_NOT_PROFILE] = true;

		param_map[PAYMENT_PARAM_PAYMENT_SCRIPT] = "/mancgi/payssionpayment";
	}
};

RUN_MODULE(Payssion)

