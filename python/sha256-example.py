import hashlib

# We talked very briefly about cryptographic hash functions.
# sha256 is a very commonly used cryptographic hash function,
# and we usually display the hash value as a hex encoded
# sequence of bytes.

# Python has an implmentation of sha256 in hashlib.

stringtohash = "I'm a lumberjack and I'm ok"
print("Hashing: "+str(stringtohash))
hashvalue = hashlib.sha256(stringtohash.encode('utf-8')).hexdigest()
print("Hex sha256 hash: "+str(hashvalue))
print("If you have the 'openssl' command on your machine you can see that ")
print("    echo -n \"I'm a lumberjack and I'm ok\" | openssl sha256")
print("Will produce the exact same hash value as above.")
stringtohash += "!"
print("Changing string to : "+str(stringtohash))
hashvalue = hashlib.sha256(stringtohash.encode('utf-8')).hexdigest()
print("Produces a different hash: "+str(hashvalue))
