import glob

text_files=glob.glob('d:/octopy/*.txt')

def file_contents(file_name):
    f=open(file_name)
    try:
        return f.read()
    finally:
        f.close()

source = dict((file_name,file_contents(file_name)) for file_name in text_files)

def final(key,value):
    print key,value

# client
def mapfn(key,value):
      '''i = 30000000
      while i > 0:
        i -= 1
      yield "hello",1'''
      
      for line in value.splitlines():
          # Loooong time
          for word in line.split():
               yield word.lower(),1

def reducefn(key,value):
       return key,len(value)