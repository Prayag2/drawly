#ifndef ITEMPROPERTY_H
#define ITEMPROPERTY_H

class ItemProperty {
  public:
    ItemProperty();

    bool active() const;
    void activate();
    void deactivate();

  private:
    bool m_isActive {false};
};

#endif // ITEMPROPERTY_H
