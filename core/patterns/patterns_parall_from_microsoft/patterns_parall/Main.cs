using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Threading;

namespace patterns_parall
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Hello World!");
		}

		static void ProcessFile(string inputPath, string outputPath) {
			var inputLines = new BlockingCollection<string>();
			var processedLines = new BlockingCollection<string>();
			
			
		}
	}

	class BlockedQueue<T> {
		private Queue<T> _queue = new Queue<T>();
		private SemaphoreSlim _sem = new SemaphoreSlim(0, int.MaxValue);

		public void Enqueue(T data) {
			if (data == null) throw new ArgumentNullException("data");
			// insert
			lock (_queue) _queue.Enqueue(data);
			_sem.Release();
		}

		public T Dequeue() {
			_sem.Wait();
			lock(_queue) return _queue.Dequeue();
		}
	}
}
