import kotlin.system.measureTimeMillis
import kotlin.concurrent.thread

fun sumArraySequential(arr: LongArray): Long {
    var sum = 0L
    for (value in arr) {
        sum += value
    }
    return sum
}

fun sumArrayParallel(arr: LongArray, numThreads: Int = Runtime.getRuntime().availableProcessors()): Long {
    val chunkSize = arr.size / numThreads
    val partialSums = LongArray(numThreads) { 0L }
    val threads = mutableListOf<Thread>()

    for (i in 0 until numThreads) {
        val start = i * chunkSize
        val end = if (i == numThreads - 1) arr.size else (i + 1) * chunkSize
        threads.add(thread {
            for (j in start until end) {
                partialSums[i] += arr[j]
            }
        })
    }

    threads.forEach { it.join() }
    return partialSums.sum()
}

fun realtimeSum(arr: LongArray): Long {
    val methods = listOf(
        { sumArrayParallel(arr) } to "Parallel",
        { sumArraySequential(arr) } to "Sequential"
    )

    for ((func, name) in methods) {
        try {
            val time = measureTimeMillis {
                val result = func()
                println("Using $name: $result")
                return result
            }
            println("Time: ${time}ms")
            return func()
        } catch (e: Exception) {
            println("Method $name failed: ${e.message}, falling back...")
        }
    }
    throw RuntimeException("All methods failed")
}

fun main() {
    val N = 1_000_000
    val arr = LongArray(N) { (it % 100).toLong() }

    try {
        val result = realtimeSum(arr)
        println("Final result: $result")
    } catch (e: Exception) {
        println("All sum methods failed: ${e.message}")
    }
}