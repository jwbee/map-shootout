import numpy as np
import string

alphabet=list(string.digits+string.ascii_letters+string.punctuation+' ')
npa=np.array(alphabet,dtype="|U1")
ls=np.random.randint(10,20,10_000_000)
ws=np.random.choice(npa, [10_000_000,20])
for w,l in zip(ws,ls):
  print("".join(w[i] for i in range(l)))
