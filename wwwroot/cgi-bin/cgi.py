import os

env_keys = [
  "AUTH_TYPE"
  ,"CONTENT_LENGTH"
  ,"CONTENT_TYPE"
  ,"GATEWAY_INTERFACE"
  ,"HTTP_HOST"
  ,"SERVER_PORT"
  ,"SERVER_PROTOCOL"
  ,"REQUEST_METHOD"
  ,"PATH_INFO"
  ,"PATH_TRANSLATED"
  ,"SCRIPT_NAME"
  ,"QUERY_STRING"
]

def print_envs(keys):
  for key in keys:
    value = os.getenv(key)
    if value:
      print(f"<p>{key}: {value}</p>")

print("<html><body>")
print("<h1>Python Script Test</h1>")
print_envs(env_keys)
print("</body></html>")