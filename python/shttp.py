#!-*- coding:utf-8 -*-	
# 用于web中走http接口传输加解密

from Crypto import Random	
from Crypto.Cipher import PKCS1_v1_5	
from Crypto.Cipher import AES
from Crypto.Hash import SHA	
from Crypto.PublicKey import RSA	
from Crypto.Signature import PKCS1_v1_5 as pk	
from hashlib import sha1	
from urllib import urlencode	 
import base64	
import urllib	
import urllib2	
import time	  
import json	
'''
生成公钥私钥对过程：   
生成私钥：
	openssl genrsa -out rsa_private_key.pem 1024	
根据私钥生成公钥： 
	openssl rsa -in rsa_private_key.pem -out rsa_public_key.pem -pubout   
这时候的私钥还不能直接被使用，需要进行PKCS#8编码：	
	openssl pkcs8 -topk8 -in rsa_private_key.pem -out pkcs8_rsa_private_key.pem -nocrypt   
命令中指明了输入私钥文件为rsa_private_key.pem，输出私钥文件为pkcs8_rsa_private_key.pem，不采用任何二次加密（-nocrypt）	
'''
publickey = RSA.importKey(open('public.pem','r').read())
privatekey = RSA.importKey(open('pkcs8_private.pem','r').read())
encryptkey = '1234567890123456'	

class SHttp:	
		
	'''   
	post请求   
	url: 参数URL   
	values: 字典类型的参数  
	'''	
	def doPost(self,url,values):	
		req = urllib2.Request(url)	
		data = urllib.urlencode(values)	
		res = urllib2.urlopen(req, data)	
		ret = res.read()	
		return ret	

	'''''   
	get请求   
	参数URL   
	字典类型的参数   
	'''	
	def doGet(self,url,values):	
		REQUEST = url + "?" + urllib.urlencode(values)	
		ret = urllib2.urlopen(REQUEST).read()	  
		return ret	
		 
	"""   
	对齐块   
	size 16   
	999999999=>9999999997777777   
	"""	
	@staticmethod	
	def _pkcs7padding(data):	
		size = AES.block_size	
		count = size - len(data)%size	
		if count:	
			data+=(chr(count)*count)	
		return data	
	   
	"""   
	反对齐   
	"""	
	@staticmethod	
	def _depkcs7padding(data):	
		newdata = ''	 
		for c in data:	
			if ord(c) > AES.block_size:	
				newdata+=c	
		return newdata	
		
		
	'''''   
	aes加密字符串
		pkcs7padding->aes->base64
	'''	
	def aes_base64_encrypt(self,data,key):	
		cipher = AES.new(key)	
		return base64.b64encode(cipher.encrypt(self._pkcs7padding(data)))	
	
	'''
	aes解密字符串
		base64->aes->depkcs7padding
	'''
	def base64_aes_decrypt(self,data,key):	
		cipher = AES.new(key)	
		return self._depkcs7padding(cipher.decrypt(base64.b64decode(data)))			
			
	'''''   
	rsa加密   
		rsa->base64
	'''	
	def rsa_base64_encrypt(self,data,key):	
		cipher = PKCS1_v1_5.new(key)	 
		return base64.b64encode(cipher.encrypt(data))	
		
	'''''   
	rsa解密   
		base64->rsa
	'''	
	def rsa_base64_decrypt(self,data,key):	
		cipher = PKCS1_v1_5.new(key)	
		return cipher.decrypt(base64.b64decode(data), Random.new().read(15+SHA.digest_size))	
			
	'''''   
	RSA签名   
	'''	
	def sign(self,signdata):	
		h=SHA.new(signdata)	
		signer = pk.new(privatekey)	
		signn=signer.sign(h)	
		signn=base64.b64encode(signn)	
		return  signn	
		   
	'''''   
	RSA验签   
	结果：如果验签通过，则返回The signature is authentic   
		 如果验签不通过，则返回"The signature is not authentic."   
	'''	
	def checksign(self,rdata):	
		signn=base64.b64decode(rdata.pop('sign'))	
		signdata=self.sort(rdata)	
		verifier = pk.new(publickey)	
		if verifier.verify(SHA.new(signdata), signn):	
			return  True
		else:	
			return False
			
		
	'''''   
	作用类似与java的treemap,   
	取出key值,按照字母排序后将value拼接起来   
	返回字符串   
	'''	
	def sort(self,mes):	
		_par = []	
		   
		keys=mes.keys()	
		keys.sort()	
		for v in keys:	
			_par.append(str(mes[v]))	
		sep=''	
		message=sep.join(_par)	
		return message	
		
	'''''   
	请求接口前的加密过程   
	'''	
	def result_encrypt(self,mesdata):	
		'''''   
		加密过程：   
		1、将需要的参数mes取出key排序后取出value拼成字符串signdata   
		2、用signdata对商户私钥进行rsa签名，生成签名signn，并转base64格式   
		3、将签名signn插入到mesdata的最后生成新的data   
		4、用encryptkey16位常量对data进行AES加密后转BASE64,生成机密后的data   
		5、用对方公钥publickey对encryptkey16位常量进行RSA加密BASE64编码，生成加密后的encryptkey   
		'''	
		signdata=self.sort(mesdata)	
		#print '需要签名的排序后的字符串为：'+signdata		   
		signn=self.sign(signdata)	
		mesdata['sign']=signn	
		#print mesdata	
		data=self.aes_base64_encrypt(json.dumps(mesdata),encryptkey)	
		#print '加密后的data='+data	
		values={}	
		values['data']=data	
		values['encryptkey']=self.rsa_base64_encrypt(encryptkey,publickey)	
		return values	
		
		
	'''''   
	对返回结果进行解密后输出   
	'''	
	def result_decrypt(self,result):	
		'''''   
		1、返回的结果json传给data和encryptkey两部分，都为加密后的   
		2、用商户私钥对encryptkey进行RSA解密，生成解密后的encryptkey。参考方法：rsa_base64_decrypt   
		3、用解密后的encryptkey对data进行AES解密。参考方法：base64_aes_decrypt   
		'''	
		kdata=result['data']	
		kencryptkey=result['encryptkey']	
		#print '返回的加密后的data='+kdata	
		#print '返回的加密后的encryptkey='+kencryptkey	
		cryptkey=self.rsa_base64_decrypt(kencryptkey,privatekey)	
		#print '解密后的encryptkey='+cryptkey	
		rdata=self.base64_aes_decrypt(kdata,cryptkey)	
		#print '解密后的data='+rdata	
		rdata=json.loads(rdata)  
		if self.checksign(rdata) == True:
			return rdata 	
		else:
			return {}
	   
	'''''   
	加密过程：   
	1、将需要的参数data取出key排序后取出value拼成字符串signdata   
	2、用signdata对商户私钥进行rsa签名，生成签名signn，并转base64格式   
	3、将签名signn插入到data的最后生成新的data   
	4、用encryptkey16位常量对data进行AES加密后转BASE64,生成机密后的data   
	5、用对方公钥publickey对encryptkey16位常量进行RSA加密BASE64编码，生成加密后的encryptkey   
	6、第四部加密后的data，第五步加密后的encryptkey作为参数post请求给URL  http://xxxx/xxx/api/xxx/xxx/xxx/xxx   
	7、返回的结果json传给data和encryptkey两部分，都为加密后的   
	8、用商户私钥对encryptkey进行RSA解密，生成解密后的encryptkey。参考方法：rsa_base64_decrypt   
	9、用解密后的encryptkey对data进行AES解密。参考方法：base64_aes_decrypt   
	'''
	def doSPost(self,url,data):				#data {"cmd":"ls"}	
		values=self.result_encrypt(data)		#values {"encrypkey":"","data":"密文"}
		result=json.loads(self.doPost(url, values))	#result {"encrypkey":"","data":"密文"}
		return self.result_decrypt(result)  		#返回 {"cmd":"ls"}

if __name__=='__main__':	
	h=SHttp()	
	result = h.doSPost("http://xxx",{"cmd":"ls"})
	
