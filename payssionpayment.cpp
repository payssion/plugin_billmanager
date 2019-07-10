#include <payment/paymentcgi.h>
#include <mgr/mgrrpc.h>
#include <mgr/mgrdate.h>
#include <mgr/mgrhash.h>
#include <mgr/mgrstream.h>
#include <sstream>

MODULE("payssionpayment");

using namespace payment;

class PayssionPayment : public PaymentCgi {
public:
	PayssionPayment() : PaymentCgi("payssion", "payssionpayment")
	{

	}

	virtual bool ValidateAuth() {
		return true;
	}

	virtual void Process() {
		StringMap payment_form_param;
		payment_form_param["source"] = "billmanager";
		payment_form_param["notify_url"] = GetServerUrl()  + "/mancgi/payssionresult";
		payment_form_param["success_url"] = GetSuccessPage();
		payment_form_param["fail_url"] = GetFailPage();
		payment_form_param["api_key"] = Method("api_key");
		payment_form_param["pm_id"] = Method("name");
		payment_form_param["order_id"] = Payment("id");
		payment_form_param["currency"] = str::Upper(Currency("iso"));
		payment_form_param["amount"] = Payment("amount");
		payment_form_param["description"] = Payment("description");

		string sign_str = payment_form_param["api_key"] + "|" +
				payment_form_param["pm_id"] + "|" +
				payment_form_param["amount"] + "|" +
				payment_form_param["currency"] + "|" +
				payment_form_param["order_id"] + "|" +
				Method("secret_key");

		payment_form_param["sign_str"] = sign_str;
		string sign = str::base64::Encode(mgr_hash::md5(sign_str));

		string form_str = "<html>\n"
						"<head>"
						"	<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'/>\n"
						"	<link rel='shortcut icon' href='billmgr.ico' type='image/x-icon'/>\n"
						"	<script language='JavaScript'>\n"
						"		function DoSubmit() {\n"
						"			document.payssionform.submit();\n"
						"		}\n"
						"	</script>\n"
						"</head>\n"
						"<body onload='DoSubmit()'>\n"
						"	<form name='payssionform' action='http://sandbox.payssion.com/payment/create.html' target='_blank' method='post'>\n"
						"		<input type='hidden' name='api_sig' value='" + sign + "'>\n";

		ForEachI(payment_form_param, p) {
			str::inpl::Append(form_str,
						"		<input type='hidden' name='" + p->first + "' value='" + p->second + "'>", "\n");
		}

		form_str +=		"	</form>\n"
						"</body>\n"
						"</html>\n";

		std::cout << form_str;
	}
};

RUN_MODULE(PayssionPayment)
