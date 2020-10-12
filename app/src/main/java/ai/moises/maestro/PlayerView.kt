package com.admqueiroga.maestro

import com.admqueiroga.maestro.databinding.PlayerLiteBinding
import android.content.Context
import android.util.AttributeSet
import android.util.TypedValue
import android.view.View
import android.view.ViewGroup
import androidx.cardview.widget.CardView

/**
 * Created by Ademir Queiroga <ademir@fuze.cc> on 05/05/21.
 */
class PlayerView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : CardView(context, attrs, defStyleAttr) {

    private var viewBinding: PlayerLiteBinding? = null

    init {
        val padding =
            TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 16f, resources.displayMetrics)
                .toInt()
        setContentPadding(padding, padding, padding, padding)
        layoutParams = LayoutParams(LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        inflate(context, R.layout.player_lite, this)
        viewBinding = PlayerLiteBinding.bind(this)
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        viewBinding = null
    }

    fun bind(player: Player, onPlayPauseClick: (Player) -> Boolean) = viewBinding?.run {
        playerTitle.text = player.track
        viewBinding?.playerPlayButton?.run {
            isSelected = true
            setOnClickListener {
                isSelected = onPlayPauseClick(player)
            }
        }
    }

}