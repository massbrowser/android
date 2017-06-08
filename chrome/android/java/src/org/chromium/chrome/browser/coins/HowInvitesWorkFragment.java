package org.chromium.chrome.browser.coins;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.app.FragmentManager;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import org.chromium.chrome.R;

/**
 * Created by elvis on 21.03.17.
 */
public class HowInvitesWorkFragment extends DialogFragment {
    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
//        getDialog().getWindow().getAttributes().windowAnimations= R.style.HowInvitesWorkDialogAnimation;
//        int style = R.style.HowInvitesWorkDialogAnimation;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        Dialog dialog = super.onCreateDialog(savedInstanceState);
//        dialog.getWindow().getAttributes().windowAnimations = R.style.HowInvitesWorkDialogAnimation;
//        dialog.getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        return dialog;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.how_invites_work_dialog, container, false);
    }

    @Override
    public void onViewCreated(final View view, @Nullable Bundle savedInstanceState) {
        view.findViewById(R.id.where_spend).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new AlertDialog.Builder(getActivity())
                        .setMessage("Coming soon")
                        .setPositiveButton("OK", null)
                        .create()
                        .show();
            }
        });
    }


}
