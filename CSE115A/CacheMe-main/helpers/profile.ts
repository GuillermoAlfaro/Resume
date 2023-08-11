import { getDownloadURL, ref } from "firebase/storage";
import { storage } from "../firebase";


export default async function getProfileImage (id: string) {
    let uri = "";
    if (id != "" && id != null) {
        const gsRef = ref(storage, "profile/" + id);
        await getDownloadURL(gsRef).then( (url) => {
            uri = url;
            return url;
        });
        return uri;
    }  else {
        return uri;
    } 
  }

