#include <payment/paymentcgi.h>
#include <mgr/mgrrpc.h>
#include <mgr/mgrdate.h>
#include <mgr/mgrstream.h>
#include <payment/paymentutil.h>
#include <mgr/mgrhash.h>
#include <sstream>

MODULE("payssionresult");

using namespace payment;

class PayssionResult : public PaymentCgi {
public:
	PayssionResult() : PaymentCgi("payssion", "payssionresult")
	{

	}

	virtual string ContentType()
	{
		return "text/xml";
	}

	virtual string ElidParam()
	{
		return "order_id";
	}

	virtual bool ValidateAuth() {
		return false;
	}

	virtual void Process() {
		if (Input("amount") != Payment("amount")) {
			throw mgr_err::Value("amonut");
		}

		if (Input("state") == "completed") {
			if (Input("paid") != Payment("amount")) {
				throw mgr_err::Value("paid");
			}
			string sign = Input("notify_sig");
			string sign_str = Method("api_key") + "|" +
					Input("pm_id") + "|" +
					Input("amount") + "|" +
					Input("currency") + "|" +
					Input("order_id") + "|" +
					Input("state") + "|" +
					Method("secret_key");
			string sign_verify = str::base64::Encode(mgr_hash::md5(sign_str));

			if (sign != sign_verify) {
				throw mgr_err::Value("signature");
			} else {
				SetPaid(MapToStr(input_params, "\n", ":\t"));
			}
		}
	}
};

RUN_MODULE(PayssionResult)
