package com.admqueiroga.maestro

import com.admqueiroga.maestro.databinding.ActivityMainBinding
import android.content.Context
import android.content.res.AssetManager
import android.media.AudioManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.View
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import kotlin.reflect.KClass

class MainActivity : AppCompatActivity() {

    lateinit var viewBinding: ActivityMainBinding

    private val maestroUiHandler = Handler(Looper.getMainLooper())

    private var seeking = false;

    private val progressUpdateRunnable = object : Runnable {
        override fun run() {
            if (isReady() && !seeking) {
                viewBinding.maestroSeekBar.progress = currentProgress()
                Log.d("Main", "current progress: ${currentProgress()}")
            } else {
                Log.d("Main", "not ready")
            }
            maestroUiHandler.postDelayed(this, 1000)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        viewBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(viewBinding.root)

        val audioManager = getSystemService(Context.AUDIO_SERVICE) as? AudioManager
        val sampleRate =
            audioManager?.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)?.toInt() ?: -1
        val bufferSize =
            audioManager?.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER)?.toInt() ?: -1
        Log.d("Main", "Starting maestro")
        startMaestro(assets, sampleRate, bufferSize)
        Log.d("Main", "STARTED maestro")

        val onPlayPauseClick: (Player) -> Boolean = { player ->
            isReady() && togglePlayback(player)
        }

        val player1 = Player("BASS.mp3")
        viewBinding.player1.bind(player1, onPlayPauseClick)
        addPlayer(player1)

        val player2 = Player("BEATS.mp3")
        viewBinding.player2.bind(player2, onPlayPauseClick)
        addPlayer(player2)

        val player3 = Player("DRUMS.mp3")
        viewBinding.player3.bind(player3, onPlayPauseClick)
        addPlayer(player3)

        val player4 = Player("OTHER.mp3")
        viewBinding.player4.bind(player4, onPlayPauseClick)
        addPlayer(player4)

        val player5 = Player("PIANO.mp3")
        viewBinding.player5.bind(player5, onPlayPauseClick)
        addPlayer(player5)

        val player6 = Player("VOCALS.mp3")
        viewBinding.player6.bind(player6, onPlayPauseClick)
        addPlayer(player6)

        Log.d("Main", "Preparing maestro")
        prepare()
        Log.d("Main", "PREPARED maestro")

        viewBinding.maestroPlayPause.setOnClickListener {
            if (isReady()) {
                if (isPlaying()) {
                    pause()
                } else {
                    play()
                }
            }
        }

        viewBinding.maestroSeekBar.setOnSeekBarChangeListener(object :
            SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {}

            override fun onStartTrackingTouch(seekBar: SeekBar) {
                seeking = true
            }

            override fun onStopTrackingTouch(seekBar: SeekBar) {
                seeking = false
                seek(seekBar.progress)
            }

        })

    }

    override fun onResume() {
        super.onResume()
        maestroUiHandler.post(progressUpdateRunnable)
    }

    override fun onStop() {
        super.onStop()
        maestroUiHandler.removeCallbacksAndMessages(null)
    }

    override fun onDestroy() {
        super.onDestroy()
        stop()
    }

    private external fun startMaestro(
        assetManager: AssetManager,
        sampleRate: Int,
        bufferSize: Int
    ): Int

    private external fun addPlayer(player: Player): String

    private external fun isPlaying(): Boolean

    private external fun isTrackPlaying(player: Player): Boolean

    private external fun play()

    private external fun playOnly(player: Player)

    private external fun pause()

    private external fun pauseOnly(player: Player)

    private external fun stop()

    private external fun seek(progress: Int)

    private external fun togglePlayback(player: Player): Boolean

    private external fun currentProgress(): Int

    private external fun isReady(): Boolean

    private external fun prepare()

    companion object {
        // Used to load the 'maestro' library on application startup.
        init {
            System.loadLibrary("maestro")
        }
    }
}
