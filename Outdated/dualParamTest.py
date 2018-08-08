import httplib2
import json

json_input = {}
merged_data = {}

def handleValues(json_input1, json_input2):
    
    
    # Set node id and value as required
    node_id1          = '@proj5raw'           # note: customId's start with '@' 
    node_id2          = '@proj5weight'
    node_value1       = json_input1 # First value (raw value)
    #print(node_value1)
    node_value2       = json_input2 # Second Value (real weight)
    #print(node_value2)
    
    # Set api key and resource endpoint
    api_key           = 'ZIhkkr1OU4NFJJfTtb8uu6FSQXZHz8bnHfonTIcp'   # you can generate an API key from account settings
    api_path          = 'https://api.eagle.io/api/v1/'
    api_resource1      = 'nodes/' + node_id1 + '/historic/now'
    #print(api_resource1)
    api_resource2      = 'nodes/' + node_id2 + '/historic/now'
    #print(api_resource2)
    
    # Build http request 1
    req_uri1           = api_path + api_resource1
    #print(req_uri1)
    req_uri2           = api_path + api_resource2
    #print(req_uri2)
    req_method        = 'PUT'    
    req_headers       = {'Content-Type': 'application/json', 'X-Api-Key': api_key}
    req_body1         = {'value': node_value1}  # optionally include timestamp and quality:
                      # {'value': 15, 'timestamp': '2017-07-14T23:38:00Z', 'quality': 149}
    
    req_body2         = {'value': node_value2}  # optionally include timestamp and quality:
                      # {'value': 15, 'timestamp': '2017-07-14T23:38:00Z', 'quality': 149}

    # Send http request 1 and get response
    http              = httplib2.Http()
    response, content1 = http.request(req_uri1, req_method, json.dumps(req_body1), req_headers)
    data1 = json.loads(content1)
    print(data1)
    
    # Send http request 2 and get response
    response, content2 = http.request(req_uri2, req_method, json.dumps(req_body2), req_headers)
    data2 = json.loads(content2)
    print(data2)

    merged_data['operation_raw'] = data1
    merged_data['operation_weight'] = data2

    return merged_data

def lambda_handler(json_input, context):
    
    json_input['value'] = [111111,222]

    # Parse for the two values
    json_input1 = json_input['value'][0]
    json_input2 = json_input['value'][1]
    
    # Call function to send the two values to EagleIO
    
    data = handleValues(json_input1, json_input2)
    print(data)
    
    return data

lambda_handler(json_input,0)