package org.chromium.chrome.browser.coins;

import android.app.DialogFragment;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.support.annotation.IdRes;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.Transformation;
import android.view.animation.TranslateAnimation;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;
import org.chromium.chrome.R;

import java.text.DecimalFormat;

/**
 * Created by elvis on 27.11.16.
 */
public class CoinsDialogFragment extends DialogFragment {
    private RadioButton masscoin;
    private RadioButton bitcoin;
    private RadioButton dollar;

    private CoinsSingleton.ChangeListener changeListener;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.coins_dialog, container, false);
    }

    @Override
    public void onViewCreated(final View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        RadioGroup radioGroup = (RadioGroup) view.findViewById(R.id.coin_group);

        masscoin = (RadioButton) view.findViewById(R.id.masscoin);
        masscoin.setTag(CoinType.MASS_COIN);


        bitcoin = (RadioButton) view.findViewById(R.id.bitcoin);
        bitcoin.setTag(CoinType.BIT_COIN);


        dollar = (RadioButton) view.findViewById(R.id.dollar);
        dollar.setTag(CoinType.DOLLAR);

        long useTime = CoinsSingleton.getInstance().getUseTime();
        ((TextView) view.findViewById(R.id.use_time)).setText(getString(R.string.use_time_text, useTime));

        updateCoins();


        view.findViewById(R.id.refer_friend).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String refer = "https://play.google.com/store/apps/details?id=network.mass.browser&referrer="
                        + Settings.Secure.getString(getActivity().getContentResolver(), Settings.Secure.ANDROID_ID);
                Intent intent = new Intent(Intent.ACTION_SEND);
                intent.setType("text/plain");
                intent.putExtra(Intent.EXTRA_SUBJECT, "Send invitation");
                intent.putExtra(Intent.EXTRA_TEXT, refer);
                startActivity(Intent.createChooser(intent, "Request send invitation"));


//                ClipboardManager clipboard = (ClipboardManager) getActivity().getSystemService(Context.CLIPBOARD_SERVICE);
//                ClipData clip = ClipData.newPlainText("", address)
//                clipboard.setPrimaryClip(clip);
//                Toast.makeText(getActivity(), getString(R.string.copied_to_clipboard), Toast.LENGTH_LONG).show()
            }
        });

        radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, @IdRes int viewId) {
                CoinType type = (CoinType) radioGroup.findViewById(viewId).getTag();
                CoinsSingleton.getInstance().setCurrentType(type);
            }
        });


        CoinsSingleton.getInstance().addChangeListener(changeListener = new CoinsSingleton.ChangeListener() {
            @Override
            public void onTypeChanged(CoinType newType) {
            }

            @Override
            public void onValueChanged(float oldValue, float newValue) {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        updateCoins();
                    }
                });
            }
        });

        view.findViewById(R.id.how_invite_work).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new HowInvitesWorkFragment().show(getFragmentManager(), null);
            }
        });
    }

    private void updateCoins() {
        DecimalFormat decimalFormat = new DecimalFormat(CoinType.MASS_COIN.getFormat());
        masscoin.setText(decimalFormat.format(CoinsSingleton.getInstance().getValue(CoinType.MASS_COIN)));
        masscoin.setChecked(CoinsSingleton.getInstance().getCurrentType() == CoinType.MASS_COIN);

        decimalFormat = new DecimalFormat(CoinType.BIT_COIN.getFormat());
        bitcoin.setText(decimalFormat.format(CoinsSingleton.getInstance().getValue(CoinType.BIT_COIN)));
        bitcoin.setChecked(CoinsSingleton.getInstance().getCurrentType() == CoinType.BIT_COIN);

        decimalFormat = new DecimalFormat(CoinType.DOLLAR.getFormat());
        dollar.setText(decimalFormat.format(CoinsSingleton.getInstance().getValue(CoinType.DOLLAR)));
        dollar.setChecked(CoinsSingleton.getInstance().getCurrentType() == CoinType.DOLLAR);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        CoinsSingleton.getInstance().removeChangeListener(changeListener);
    }
}
