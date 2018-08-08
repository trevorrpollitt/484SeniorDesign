import httplib2
import json

def lambda_handler(json_input, context):
    # Set node id and value as required
    node_id           = '@proj5'           # note: customId's start with '@' 
    node_value        = json_input # generate random number between 0-100 for this example
    
    # Set api key and resource endpoint
    api_key           = 'ZIhkkr1OU4NFJJfTtb8uu6FSQXZHz8bnHfonTIcp'   # you can generate an API key from account settings
    api_path          = 'https://api.eagle.io/api/v1/'
    api_resource      = 'nodes/' + node_id + '/historic/now'
    
    # Build http request
    req_uri           = api_path + api_resource
    req_method        = 'PUT'    
    req_headers       = {'Content-Type': 'application/json', 'X-Api-Key': api_key}
    req_body          = {'value': node_value}  # optionally include timestamp and quality:
                      # {'value': 15, 'timestamp': '2017-07-14T23:38:00Z', 'quality': 149}
    
    # Send http request and get response
    http              = httplib2.Http()
    response, content = http.request(req_uri, req_method, json.dumps(req_body), req_headers)

    # Parse content as json
    data              = json.loads(content)

    return data

data = lambda_handler(10,0)
print(data)