Text Length :
 Short text case - All three metrics will struggle, size is too small to match the English distribution.
 Long text case - Accuracy is higher, it will be closer to the English distribution.

Chi-Squared Distance: Divides by expected frequency making rare letters reach a much more accurate result.
Euclidean Distance: It treats all letters differences equally. A spike in "E" is treated the same as one in "X".
Cosine Distance: Measures angle between vectors, struggles on short text.

Cases of failure of Frequency Analysis:
If one encrypts a message which intentionally avoids the letter "E", it will most likely fail. It expects "E" to be the peak.
A message like "hahahahahah" has only two letters. The distance metrics will find too many low distance shifts.
Code or mathematical English (using X, Y, Z for example) will often confuse the Euclidean distance for example.
