import * as ImagePicker from 'expo-image-picker';
import * as ImageManipulator from 'expo-image-manipulator';
import { ref, uploadBytes } from "firebase/storage"
import { TouchableOpacity, StyleSheet } from 'react-native';
import RegularText from '../Texts/regularText';
import { db, storage } from '../../firebase';
import { doc, updateDoc } from 'firebase/firestore';
import uuid from 'react-native-uuid';

type ChangeProfilePictureProps = {
  userID: string;
  changeProfilePictureID: (id: string) => void;

}

export default function ChangeProfilePicture(props: ChangeProfilePictureProps) {
const uploadImage = async (image: { uri: any; width?: number; height?: number; base64?: string | undefined; }) => { 
  if(image){
    
    const imageID = uuid.v1().toString();
    const response = await fetch(image.uri);
    const blobFile = await response.blob();
    const imageRef = ref(storage, `profile/${imageID}`);
    await updateDoc(doc(db, "user", props.userID), {profilePicture: imageID,});
    await uploadBytes(imageRef, blobFile).then(() =>{
      props.changeProfilePictureID(imageID);
      // Delete old profile picture from storage
  });
  }
}
const pickImage = async () => {
      let result = await ImagePicker.launchImageLibraryAsync({
        mediaTypes: ImagePicker.MediaTypeOptions.All,
        allowsEditing: true,
        aspect: [4, 3],
        quality: 1,
        base64: true, // Remove?
      });
      
      if (!result.canceled) {
        await ImageManipulator.manipulateAsync(
          result.assets[0].uri, [{resize: {width: 500}}], {compress: 1}).then((image) => {
          uploadImage(image);
        })
      }
    };

    return (
        <TouchableOpacity
        onPress={pickImage}
        style={styles.button}>
            <RegularText style={styles.buttonText}>Edit Profile</RegularText>
        </TouchableOpacity>
    )
}


const styles = StyleSheet.create({
    container: {
      flex: 1,
      justifyContent: 'center',
      alignItems: 'center'
    },
     button: {
      width: 90,
      height: 30,
      marginTop: 10,
      flexDirection: "row",
      alignItems: 'center',
      justifyContent: 'center',
      backgroundColor: 'white',
      borderRadius: 10,
    },
    buttonText: {
      color: 'black',
      fontWeight: '700',
      fontSize: 12,
    },
  })