{
	"service" : {
		"api" : "http",
		"port" : 4224
	},
	"http" : {
		"script_names" : ["/shanghainesedict"]
	},
	"localization" : {
		"encoding" : "utf-8",
		"messages" : {
			"paths" : [ "../locale" ],
			"domains" : [ "hello" ]
		},
		"locales" : [ "en_GB.UTF-8", "fr_FR.UTF-8" ]
	},
    "session" : {
        "expire" : "renew",
        "timeout" : 604800,
        "location" : "server",
        "server" : {
            "storage" : "memory"
        }
    },
	"shanghainesedict" : {
        "dictxml" : "../doc/shdict.xml",
        "dictxml2" : "../doc/cfdict.xml",
		"css" : "/css",
		"img" : "/img",
        "web" : "http://127.0.0.1:4224/",
        "languages" : {
            "eng" : "en_GB.UTF-8",
            "fra" : "fr_FR.UTF-8"
        }
	}
}
