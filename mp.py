from datetime import datetime
from multiprocessing import Queue, SimpleQueue, Manager, Process
from multiprocessing.managers import SyncManager
from queue import PriorityQueue
import os

class MyManager(SyncManager):
  pass
MyManager.register('PriorityQueue', PriorityQueue)

def get_manager():
  m = MyManager()
  m.start()
  return m

def get_queue(m):
  #return m.PriorityQueue()
  return SimpleQueue()
  #return Queue()

def log(*args):
  print(os.getpid(), datetime.now(), *args)

def a():
  log('A starts')

  with get_manager() as m:
    q = get_queue(m)
    def b():
      log('B starts')
      while True:
        i = q.get()
        if i == 7:
          log('B ends')
          return

    ps = [Process(target=b) for i in range(64)]
    for p in ps:
      p.start()
    for i in range(100000):
      q.put(13)
    for p in ps:
      q.put(7)
    for p in ps:
      p.join()

  log('A ends')

if __name__ == '__main__':
  a()
