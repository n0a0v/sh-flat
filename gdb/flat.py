#!/usr/bin/python3

"""Pretty-printer for sh::flat_map, sh::unordered_flat_map,
sh::adjacent_flat_map, and sh::unordered_adjacent_flat_map."""

import gdb

class VectorPrinter:
	"""Pretty-printer utilities to help extract from std::vector."""

	@staticmethod
	def data(vector):
		"""Extract data from a vector-like container."""
		# Use the data() member function.
		try:
			return int(vector['data']())
		except gdb.error:
			pass
		# libstdc++ vector internal layout.
		try:
			impl = vector['_M_impl']
			start = impl['_M_start']
			return start
		except gdb.error:
			pass
		# libc++ vector internal layout.
		try:
			begin = vector['__begin_']
			return begin
		except gdb.error:
			pass
		print('vector container data() inaccessible')
		return None
	@staticmethod
	def size(vector):
		"""Extract size from a vector-like container."""
		# Use the size() member function.
		try:
			return int(vector['size']())
		except gdb.error:
			pass
		# libstdc++ vector internal layout.
		try:
			impl = vector['_M_impl']
			start = impl['_M_start']
			finish = impl['_M_finish']
			return finish - start
		except gdb.error:
			pass
		# libc++ vector internal layout.
		try:
			begin = vector['__begin_']
			end = vector['__end_']
			return end - begin
		except gdb.error:
			pass
		print('vector container size() inaccessible')
		return -1

class FlatMapPrinter:
	"""Pretty-printer for sh::flat_map and sh::unordered_flat_map."""

	def __init__(self, typename, value):
		"""Extract keys & values containers from flat_map."""
		self.typename = typename
		self.keys = VectorPrinter.data(value['m_keys'])
		self.values = VectorPrinter.data(value['m_values'])
		self.size = VectorPrinter.size(value['m_keys'])

	def to_string(self):
		"""Return a brief description of the flat_map."""
		return f'{self.typename} with {self.size} elements'

	def children(self):
		"""Return the children of the flat_map."""
		for index in range(self.size):
			key, value = self.keys[index], self.values[index]
			yield f'[{key}]', value

class AdjacentFlatMapPrinter:
	"""Pretty-printer for sh::adjacent_flat_map and sh::unordered_adjacent_flat_map."""

	def __init__(self, typename, value):
		"""Extract keys & values containers from adjacent_flat_map."""
		self.typename = typename
		self.data = VectorPrinter.data(value['m_key_value_pairs'])
		self.size = VectorPrinter.size(value['m_key_value_pairs'])

	def to_string(self):
		"""Return a brief description of the adjacent_flat_map."""
		return f'{self.typename} with {self.size} elements'

	def children(self):
		"""Return the children of the adjacent_flat_map."""
		for index in range(self.size):
			key, value = self.data[index]['first'], self.data[index]['second']
			yield f'[{key}]', value

def build_pretty_printer():
	"""Build & return a pretty printer collection."""
	pp = gdb.printing.RegexpCollectionPrettyPrinter('sh::flat_map')
	pp.add_printer(
		'sh::flat_map',
		'^sh::flat_map<.*>$',
		lambda value: FlatMapPrinter('sh::flat_map', value)
	)
	pp.add_printer(
		'sh::unordered_flat_map',
		'^sh::unordered_flat_map<.*>$',
		lambda value: FlatMapPrinter('sh::unordered_flat_map', value)
	)
	pp.add_printer(
		'sh::adjacent_flat_map',
		'^sh::adjacent_flat_map<.*>$',
		lambda value: AdjacentFlatMapPrinter('sh::adjacent_flat_map', value)
	)
	pp.add_printer(
		'sh::unordered_adjacent_flat_map',
		'^sh::unordered_adjacent_flat_map<.*>$',
		lambda value: AdjacentFlatMapPrinter('sh::unordered_adjacent_flat_map', value)
	)
	return pp

def register_flat_map_printers(objfile=None):
	"""Register the pretty printer collection."""
	gdb.printing.register_pretty_printer(objfile, build_pretty_printer(), replace=True)

register_flat_map_printers(gdb.current_objfile())
