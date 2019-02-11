#!/bin/bash
checkEnv () {
    if test ! -d "./body"; then
          mkdir body
    fi

    if test ! -d "./tattoo"; then
          mkdir tattoo
    fi

    if test ! -d "./decoration"; then
          mkdir decoration
    fi   

    if test ! -d "./hands"; then
          mkdir hands
    fi

    if test ! -d "./feet"; then
          mkdir feet
    fi

    if test ! -d "./eyes"; then
          mkdir eyes
    fi    
}

stripBody() {
    cp $f "./body"

    cd "./body"

    convert $f -crop 96x96+0+0 output.png
    convert $f -crop 96x96+96+0 output2.png

    montage -geometry +1+1 output2.png output.png -background none output3.png

    mogrify -extent 384x96+0+0 -background none output3.png
    
    cp output3.png $f

    rm output.png output2.png output3.png

    cd ..
}

stripEyes() {
    cp $f "./eyes"

    cd "./eyes"

    convert $f -crop 576x32+64+96 output.png

    cp output.png $f

    rm output.png

    cd ..
}

stripFeet() {
    cp $f "./feet"

    cd "./feet"

    convert $f -crop 64x64+192+32 output.png

    cp output.png $f

    rm output.png

    cd ..
}

stripHands() {
    cp $f "./hands"

    cd "./hands"

    convert $f -crop 64x32+192+0 output.png

    cp output.png $f

    rm output.png

    cd ..
}

createSkin() {
    name=$(echo $f | cut -f 1 -d '.')
    touch "$name.skn"

    echo "($f, body_hue, body_sat, body_lgt,
        $f, tatto_hue, tattoo_sat, tattoo_lgt, tattoo_alp
        $f, decoration_hue, decoration_sat, decoration_lgt,
        $f, hands_hue, hands_sat, hands_lgt,
        $f, feet_hue, feet_sat, feet_lgt,
        $f, eyes_hue, eyes_sat, eyes_lgt)" >> $name.skn
}

deleteSkin() {
    rm $f
}

checkEnv

for f in *.png; do
    stripBody
    stripEyes
    stripFeet
    stripHands
    createSkin
    #deleteSkin
done
