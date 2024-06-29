#!/usr/bin/python3

"""Accept input from sh-flat benchmark program output and translate into CSV."""

import re
import sys

class Source:
	"""Input iterator over lines with the option to push lines back onto the front of the queue."""
	def __init__(self, lines):
		self.lines = lines
		self.iter = iter(self.lines)
		self.next = []
		self.count = 0
	def __iter__(self):
		return self
	def __next__(self):
		self.count += 1
		if self.next:
			return self.next.pop()
		return next(self.iter)
	def push(self, item):
		"""Place an item back onto the queue as the next to be returned."""
		self.count -= 1
		self.next.append(item)

class Sink:
	"""Output sink of values."""
	def __init__(self):
		self.labels = []
		self.values = {}
		self.frozen = False
	def push(self):
		"""Base implementation that discards all values and marks this Sink as frozen."""
		self.values = {}
		self.frozen = True
	def append_value(self, label, value):
		"""Add a labeled value to this Sink."""
		if label not in self.labels:
			assert not self.frozen
			self.labels.append(label)
		assert label not in self.values
		self.values[label] = value
	def close(self):
		"""If values are cached, flush them."""
		if self.values:
			self.push()

class PrintSink(Sink):
	"""Output sink of values that prints CSV formatted text."""
	def push(self):
		"""Override implementation that prints all values, discards them, then
		   marks this Sink as frozen."""
		if self.values:
			if not self.frozen:
				for index, label in enumerate(self.labels):
					print(label.replace(',',''), end=(',' if index+1 < len(self.labels) else ''))
				print()
			for index, label in enumerate(self.labels):
				print(self.values[label], end=(',' if index+1 < len(self.labels) else ''))
			print()
		super().push()

class Parser:
	"""Recursive descent parser for the sh-flat benchmark program output."""
	GROUP = re.compile(r"^([^\t].*)\n$")
	LABEL = re.compile(r"^\t([^\t][^<]*)(<[^>]+>)\n$")
	ENTRY = re.compile(r"^\t\t([a-z]+): (.*) ns\n$")
	def __init__(self, source, sink):
		self.source = source
		self.sink = sink
	def parse_entry(self, label, entry, value):
		"""Parse entries (group -> label -> entry)."""
		if entry == 'avg':
			try:
				self.sink.append_value(label, value)
			except AssertionError:
				print(f'Error during append_value on line {self.source.count} with:\n'
				      f'"\t{label}",\n'
				      f'\t"{entry}",\n'
				      f'\t"{value}".',
				      file=sys.stderr)
				raise
	def parse_label(self, label):
		"""Parse labels (group -> label)."""
		for line in self.source:
			entry = Parser.ENTRY.match(line)
			if entry:
				self.parse_entry(label, entry.group(1), int(entry.group(2).replace(',','')))
			else:
				self.source.push(line)
				break
	def parse_group(self):
		"""Parse groups."""
		for line in self.source:
			label = Parser.LABEL.match(line)
			if label:
				self.parse_label(label.group(1))
			else:
				self.source.push(line)
				break
	def parse(self):
		"""Parse from root."""
		for line in self.source:
			group = Parser.GROUP.match(line)
			if group:
				self.parse_group()
				self.sink.push()
			else:
				print("root ?: " + str(line))

def parse_stdin_and_output_as_csv():
	"""Parse stdin (sh-flat benchmark output) to stdout (CSV)."""
	source = Source(sys.stdin.readlines())
	sink = PrintSink()
	parser = Parser(source, sink)
	parser.parse()
	sink.close()

if __name__ == '__main__':
	parse_stdin_and_output_as_csv()
