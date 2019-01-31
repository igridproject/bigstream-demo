#!/usr/bin/env python
import sys, getopt
import requests
import base64
import json

bs_api_url='http://demo.bs.igridproject.info/api'
storage_name='test.upload'
token='ACL_TOKEN'
datafile='input.csv'

dkey='latest'
metalist=[]

def sent_to_bs(sname,pload):
    url = bs_api_url + '/storage/' + sname
    headers = {'Content-Type': "application/json",'Authorization': "Bearer " + token}
    payload = json.dumps(pload)
    resp = requests.request("PUT", url, data=payload, headers=headers)
    return resp

def mkmeta():
    meta={'_key':dkey}
    for k,v in metalist:
        meta[k] = v
    return meta

def encode_file_as_base64( filename ):
    encoded_string = ""
    with open(filename, "rb") as data_file:
        encoded_string = base64.b64encode(data_file.read())

    return encoded_string

def helpmsg():
    print 'bsupload.py -s <storagename> -k <keyname> -m <metakey:value> file'

def main(argv):
    global storage_name
    try:
        opts, args = getopt.getopt(argv,"hs:k:m:",["storage=","key="])
    except getopt.GetoptError:
        helpmsg()
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            helpmsg()
            sys.exit()
        elif opt in ("-s", "--storage"):
            storage_name = arg
        elif opt in ("-k", "--key"):
            dkey = arg
        elif opt in ("-m"):
            mstr = arg.split(':')
            if len(mstr) == 2:
                metalist.append((mstr[0],mstr[1]))
    if len(args) > 0:
        datafile = args[0]

    fmeta = mkmeta()
    fdata = {'object_type':'bsdata','data_type':'binary','encoding':'base64'}
    fdata['data'] = encode_file_as_base64(datafile)
    payload = {'meta':fmeta,'data':fdata}
    resp = sent_to_bs(storage_name,payload)
    if resp.status_code == 200:
        print '+OK'
    else:
        print '-ERR'

if __name__ == "__main__":
   main(sys.argv[1:])
