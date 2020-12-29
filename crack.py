# INIT ===================================================================
import zipfile
import sys
from tqdm import tqdm

charlist = 'abcdefghijklmnopqrstuvwxyz'
complete = []


# OPTIONS ================================================================

#length
if '-l' in sys.argv:
    index = sys.argv.index('-f') + 1
    length = sys.argv[index]
else:
    length = 4

# file
if '-f' in sys.argv:
    index = sys.argv.index('-f') + 1
    secret = sys.argv[index]
else:
    secret = "secret.txt.zip"

#target
if '-t' in sys.argv:
    index = sys.argv.index('-t') + 1
    target = sys.argv[index]
else:
    target = "secret.txt"

# silent
if '-s' not in sys.argv:
    silent = False
else:
    silent = True


# EXECUTION ================================================================

# Generate word list
for current in range(length):
    a = [ i for i in charlist ]
    for x in range(current):
        a = [ y + i for i in charlist for y in a ]
    complete = complete + a
    '''
    for x in range:
        for y in a:
            for i in charList:
    '''

# point @ zip
z = zipfile.ZipFile(secret)

# init count
tries = 0

# progress bar
if not silent:
    complete = tqdm(complete)

# brute force
for password in complete:

    try:
        tries += 1
        z.setpassword(password.encode('ascii'))
        z.extract(target)
        print(f"Password was found after {tries} tries. Password is {password}")
        break

    except KeyboardInterrupt:
        exit(0)

    except:
        pass
    
exit(0);