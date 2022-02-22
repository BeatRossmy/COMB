#ifndef SEQUENCE_h
#define SEQUENCE_h

const int LENGTH = 3;

class SEQUENCE {
  private:
    char seq[LENGTH];
    bool getStep (int index) {
      if (index>=LENGTH*6 || index<0) return false;
      return (((seq[(index/6)]>>(index%6)) & 0x01) == 0x01)?true:false;
    }

  public:
    bool getStep (int index, const PATH path) {
      return getStep(path.path[index%path.length]);
    }

    void setPart (int index, char p) {
      if (index>=LENGTH || index<0) return;
      seq[index] = p;
    }

    char getPart (int index) {
      if (index>=LENGTH || index<0) return 0x00;
      return seq[index];
    }
};

#endif
