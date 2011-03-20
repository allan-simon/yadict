{
	"service" : {
		"api" : "http",
		"port" : 4244
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
        "dictxml2" : "../doc/shdict.xml",
        "dictxml" : "../doc/cfdict.xml",
		"css" : "/css",
		"img" : "/img",
        "web" : "http://127.0.0.1:4244/",
        "languages" : {
            "wuu" : ["Shanghainese", 61],
            "eng" : ["English", 17],
            "fra" : ["French", 23],
            "deu" : ["German", 15],
            "cmn" : ["Chinese", 12],
            "yue" : ["Cantonese", 63]
        },
        "indexedMetas" : {
            "cmn" : [ "pinyin" , "traditional"]
        }
	}
}
