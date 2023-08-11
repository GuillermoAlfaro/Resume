import { StyleSheet, KeyboardAvoidingView, FlatList, Dimensions,} from 'react-native';
// import { KeyboardAwareFlatList } from 'react-native-keyboard-aware-scroll-view'
import React, {useEffect, useState} from 'react';
import { GeoPoint } from 'firebase/firestore';

import PostHeader from './PostHeader';
import CommentBar from './CommentBar';
import NewPostComment from './NewPostComment';

const windowWidth = Dimensions.get('screen').width;
const windowHeight = Dimensions.get('screen').height;

type CacheData = {
  id: string, 
  userId: string, 
  username: string,
  imageRef: string, 
  caption: string, 
  comments: string[],
  distBtwn: number, 
  timePosted: string, 
  location: GeoPoint, 
  nLikes: number,
  nComments: number,
}


const PostPage = (props: CacheData) => {
  const [comments, setComments] = useState<string[]>([]);
  const [numComments, setNumComments] = useState<number>(props.nComments);
  useEffect (() => {
    setComments(props.comments);
    setNumComments(props.nComments);
  }
  , [props.comments, props.nComments]) 
  const newComment = (newComment: string) => {
    setComments([...comments, newComment]);
    setNumComments(numComments + 1);
  }


  return (
      <KeyboardAvoidingView style={{flex: 1}}>
          <FlatList style={styles.scrollingComments}
            stickyHeaderIndices={[0]}
            ListHeaderComponentStyle={{backgroundColor: "#EEF2FF"}}
            data={comments}
            extraData={comments}
            keyExtractor={(c) => c}
            ListHeaderComponent={
              <PostHeader id={props.id} 
                userId={props.userId} 
                username={props.username}
                imageRef={props.imageRef}
                caption={props.caption}
                distBtwn={props.distBtwn} 
                timePosted={props.timePosted}
                location={props.location}
                nLikes={props.nLikes}
                nComments={numComments} />
            }
            renderItem={({item}) => {
              return (
                <NewPostComment id={item} />
              )
            }}>
          </FlatList>

        <KeyboardAvoidingView  style={styles.commentInput} behavior='padding'>
          <CommentBar 
            __id={props.id}
            addComment={newComment}/>
        </KeyboardAvoidingView>
      </KeyboardAvoidingView>
  );
};

const styles = StyleSheet.create({
  scrollingComments: {
    backgroundColor: '#EEF2FF',
    marginBottom: 0
  },
  commentInput: {
    //top: windowHeight - 90,
    //top: windowHeight - 760,
  },
});

export default PostPage;