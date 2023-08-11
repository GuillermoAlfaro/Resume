import { useState } from "react";
import { Dimensions, View, Image } from "react-native";
import getProfileImage from "../../helpers/profile";

type profilePictureProps = {
    profilePictureID: string;
}

const windowWidth = Dimensions.get('screen').width

export default function Home(props: profilePictureProps) {
    const [imageURI, setImageURI] = useState("");
    getProfileImage(props.profilePictureID).then( async (uri) => {
        if (uri) {
            setImageURI(uri);
        } else {
            setImageURI("");
        }
    });

    return (
        <View style={{
            width: windowWidth,
            justifyContent: "center",
            alignItems: "center",}}> 
                    {imageURI != "" && <Image source={{uri: imageURI}}
                    style={{
                            width: windowWidth*.2, 
                            height: windowWidth*.2, 
                            borderRadius: (windowWidth*.2)/2
                        }}
                    />}
                </View>
    )
}