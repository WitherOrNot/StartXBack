# Activate via web browser

import hashlib, base64

license_key = "00000000-0000-0000-0000-000000000000"
hwid = "4e9934f69c3fd8c3e8502a2fd1ab89c2e78671d38a9b97ba313f5eaba6fd420f"

activation_data = f"{hashlib.md5(license_key.encode('utf-8')).hexdigest()}{hwid}SABALL".ljust(128, "0")

print(base64.b64encode(activation_data.encode("utf-8")).decode())