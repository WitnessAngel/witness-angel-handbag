# API

## ping
### Request pc
- {"jsonrpc": "2.0", "method": "ping", "id": 28}
### Respond esp
- {"jsonrpc":"2.0","id":28,"result":{"default": "pong"}}

## status
### Request pc
- {"jsonrpc": "2.0", "method": "status", "id": 28}
### Respond esp
- {"jsonrpc":"2.0","id":28,"result":{"available_symkey_count":42, "cryptainer_data_count":200, "disk_free_space_bytes":20000, "disk_total_space_bytes": 50000}}

## add_aes_cbc_symkey
### Request pc
- {"jsonrpc": "2.0", "method": "add_aes_cbc_symkey", "id": 28, "params": {"cryptainer_uuid":"value-value", "symkey":{"key":"value==","iv":"value=="}}}
-> symkey value : base 64
### Respond esp
- {"jsonrpc":"2.0","id":28,"result":[]}
### error esp
- {"jsonrpc":"2.0","id":28,"error":{"name": "JSONRPCError", "code": (number 100-999), "message": "Some Error Occurred", "error": "whatever you want\n(a traceback?)"},}




# Hardware 

- Besoin d'un bouton arret de l'enregistrement.
- Besoin d'une interface etat : (ex : led, ecran lcd).
- Probleme de l'heure, avoir l'heure sur l'esp.

# SD

## ROOT

### symkey_pool

- symkey a disposition nommer par leurs __cryptainer_uuid__.

### cryptainer_data (offloaded)

- wave file chiffré nommer : {cryptainer_uuid}.crypt.data.



